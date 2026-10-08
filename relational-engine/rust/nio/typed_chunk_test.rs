//! Owns every TypedChunk public form: packed-word boundaries, stable/aligned
//! storage, removal ownership, hole reuse, rejection recovery and bounded strings.
//! Indices are not identities. Stale raw pointers/concurrent mutation are outside
//! this exclusive Rust contract; generation/type/C ABI proof belongs to later work.
#[path = "../fail_allocator.rs"] mod faults;
use relational_engine_scratchpad::{TypedChunk, StorageError};
use std::{cell::Cell, rc::Rc};
#[repr(align(256))]
struct Row(u64);
struct Tracked(Rc<Cell<usize>>);
impl Drop for Tracked { fn drop(&mut self) { self.0.set(self.0.get() + 1); } }

#[test]
fn geometry_bitmap_reuse_and_lifetime() {
    assert_eq!(TypedChunk!(u8).unwrap().get_capacity(), 1024);
    assert!(TypedChunk::<u8>::zero().unwrap().is_empty());
    assert!(matches!(TypedChunk::<()>::new(1), Err(StorageError::Layout)));
    for capacity in [0, usize::MAX, isize::MAX as usize] {
        assert!(matches!(TypedChunk::<u64>::new(capacity), Err(StorageError::Layout)));
    }
    for capacity in [1, 63, 64, 65, 127, 128, 129, 1024] {
        let mut chunk = TypedChunk!(Row, capacity).unwrap();
        assert!(chunk.is_empty() && !chunk.is_full());
        let mut pointers = Vec::new();
        for index in 0..capacity {
            assert_eq!(chunk.add(Row(index as u64)), Ok(index));
            let pointer = chunk.get(index).unwrap() as *const Row;
            assert_eq!((pointer as usize) % 256, 0);
            if index > 0 { assert_eq!(pointer as usize - pointers[index - 1] as usize, 256); }
            pointers.push(pointer);
        }
        assert!(chunk.is_full());
        assert_eq!(chunk.len(), capacity);
        assert_eq!(chunk.add(Row(999)), Err(StorageError::Capacity));
        assert!(matches!(chunk.remove(capacity), Err(StorageError::Bounds)));
        assert!(chunk.get(usize::MAX).is_none() && chunk.get_mut(capacity).is_none());
        let hole = capacity / 2;
        assert_eq!(chunk.remove(hole).unwrap().0, hole as u64);
        assert!(matches!(chunk.remove(hole), Err(StorageError::Bounds)));
        assert!(chunk.get(hole).is_none() && chunk.get_mut(hole).is_none());
        assert_eq!(chunk.len(), capacity - 1);
        assert_eq!(chunk.add(Row(999)), Ok(hole));
        for (index, pointer) in pointers.iter().copied().enumerate() {
            assert_eq!(chunk.get(index).unwrap() as *const Row, pointer);
            // No dereference of a freed pointer: this is the currently live slot.
            assert_eq!(chunk.get(index).unwrap().0, if index == hole { 999 } else { index as u64 });
        }
        chunk.get_mut(hole).unwrap().0 = 1000;
        assert_eq!(chunk.get(hole).unwrap().0, 1000);
        let mut dest = [0; 256];
        let mut truncated = true;
        assert!(chunk.to_string(&mut dest, &mut truncated) && !truncated);
        assert!(chunk.to_string_struct(&mut dest, &mut truncated));
        assert!(!chunk.to_string(&mut [], &mut truncated) && truncated);
        assert!(!chunk.to_string_struct(&mut [0; 1], &mut truncated) && truncated);
        for index in 0..capacity { chunk.remove(index).unwrap(); }
        assert!(chunk.is_empty());
        assert_eq!(chunk.add(Row(42)), Ok(0));
    }
    let drops = Rc::new(Cell::new(0));
    {
        let mut chunk = TypedChunk!(Tracked, 2).unwrap();
        chunk.add(Tracked(drops.clone())).unwrap();
        chunk.add(Tracked(drops.clone())).unwrap();
        assert_eq!(chunk.add(Tracked(drops.clone())), Err(StorageError::Capacity));
        assert_eq!(drops.get(), 1);
        let value = chunk.remove(0).unwrap();
        assert_eq!(drops.get(), 1); // Removal transfers, does not destroy.
        drop(value);
        assert_eq!(drops.get(), 2);
        chunk.add(Tracked(drops.clone())).unwrap();
    }
    assert_eq!(drops.get(), 4);
}

#[test]
fn each_allocation_stage_rejects_and_recovers() {
    for stage in [0, 1] {
        faults::fail_after(stage);
        let rejected = TypedChunk::<u64>::new(65);
        faults::reset();
        assert!(matches!(rejected, Err(StorageError::Allocation)));
        let mut chunk = TypedChunk::<u64>::new(65).unwrap();
        // Admitted storage never allocates on add/remove/reuse/get.
        faults::fail_after(0);
        let first = chunk.add(3);
        let removed = chunk.remove(0);
        let reused = chunk.add(4);
        let read = chunk.get(0).copied();
        faults::reset();
        assert_eq!(first, Ok(0));
        assert_eq!(removed, Ok(3));
        assert_eq!(reused, Ok(0));
        assert_eq!(read, Some(4));
    }
}
