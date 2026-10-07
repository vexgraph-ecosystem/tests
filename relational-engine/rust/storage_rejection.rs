//! Compile-negative fixture: each cfg selects a single intended error.
use relational_engine_scratchpad::*;
fn main() {
    #[cfg(chunk_arity)] let _ = Chunk!(u64, 2, 3);
    #[cfg(list_arity)] let _ = ChunkedList!(u64, 2, 3);
    #[cfg(slot_arity)] let _ = VariableSlot!(b"x", std::ptr::null(), 3);
    #[cfg(registry_arity)] let _ = VariableRegistry!(2, 3);
    #[cfg(wrong_capacity)] let _ = Chunk!(u64, "wrong");
    #[cfg(chunk_borrow)] {
        let mut owner = Chunk!(u64, 2).unwrap();
        owner.add(1).unwrap();
        let borrowed = owner.get(0).unwrap();
        owner.add(2).unwrap();
        println!("{borrowed}");
    }
    #[cfg(registry_borrow)] {
        let mut owner = VariableRegistry!().unwrap();
        owner.add(b"a", std::ptr::null()).unwrap();
        let borrowed = owner.get(0).unwrap();
        owner.add(b"b", std::ptr::null()).unwrap();
        println!("{:?}", borrowed.get_name());
    }
}
