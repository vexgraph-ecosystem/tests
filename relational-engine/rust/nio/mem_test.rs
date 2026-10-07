// Shared owner proof for the Rust learning backend; not Vexspoke allocator parity.
use relational_engine_scratchpad::{bytes, Memory, MemoryError};

#[test]
fn memory_lifetime_growth_and_rejection() {
    let mut memory: Memory = Memory!();
    assert!(memory.is_empty());
    assert!(Memory::default().is_empty());
    assert!(relational_engine_scratchpad::nio::mem::Memory::new().is_empty());
    assert!(relational_engine_scratchpad::mem::Memory::new().is_empty());
    let id = memory.copy_bytes(bytes!("hello 🌍")).unwrap();
    for _ in 0..1024 { memory.copy_bytes(b"other").unwrap(); }
    assert_eq!(memory.get(id).unwrap(), "hello 🌍".as_bytes());
    assert_eq!(memory.release(0), Err(MemoryError::UnknownHandle));
    assert_eq!(memory.len(), 1025);
    memory.release(id).unwrap();
    assert_eq!(memory.get(id), Err(MemoryError::UnknownHandle));
    assert_eq!(memory.release(id), Err(MemoryError::UnknownHandle));
    memory.clear();
    let next = memory.copy_bytes(b"").unwrap();
    assert!(next > id);
    assert_eq!(memory.get(next).unwrap(), b"");
    let binary = memory.copy_bytes(b"a\0b").unwrap();
    assert_eq!(memory.get(binary).unwrap(), b"a\0b");
    let mut independent = Memory::new();
    independent.copy_bytes(b"independent").unwrap();
    drop(memory);
    assert_eq!(independent.len(), 1);
}
