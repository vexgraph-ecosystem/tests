// Owner proof for ffi/memory: rejection preserves outputs, then valid calls recover.
use relational_engine_scratchpad::ffi::memory::*;

#[test]
fn ffi_rejection_preserves_outputs_and_recovers() {
    unsafe {
        relational_engine_scratchpad::ffi::re_memory_drop(std::ptr::null_mut());
        let owner = re_memory_new();
        let mut id = 99;
        assert_eq!(re_memory_copy(std::ptr::null_mut(), b"x".as_ptr(), 1, &mut id), 1);
        assert_eq!(re_memory_copy(owner, b"x".as_ptr(), 1, std::ptr::null_mut()), 1);
        assert_eq!(re_memory_copy(owner, b"x".as_ptr(), usize::MAX, &mut id), 1);
        assert_eq!(re_memory_copy(owner, std::ptr::null(), 1, &mut id), 1);
        assert_eq!(id, 99);
        assert_eq!(re_memory_copy(owner, b"hello".as_ptr(), 5, &mut id), 0);
        let mut dest = [0xaa; 5];
        let mut length = 99;
        assert_eq!(re_memory_read(std::ptr::null(), id, dest.as_mut_ptr(), 5, &mut length), 1);
        assert_eq!(re_memory_read(owner, id, dest.as_mut_ptr(), 5, std::ptr::null_mut()), 1);
        assert_eq!(re_memory_read(owner, id, dest.as_mut_ptr(), usize::MAX, &mut length), 1);
        assert_eq!(re_memory_read(owner, id, dest.as_mut_ptr(), 4, &mut length), 4);
        assert_eq!(dest, [0xaa; 5]);
        assert_eq!(length, 99);
        assert_eq!(re_memory_read(owner, 0, dest.as_mut_ptr(), 5, &mut length), 3);
        assert_eq!(re_memory_read(owner, id, std::ptr::null_mut(), 5, &mut length), 1);
        assert_eq!(re_memory_read(owner, id, dest.as_mut_ptr(), 5, &mut length), 0);
        assert_eq!(&dest, b"hello");
        assert_eq!(length, 5);
        assert_eq!(re_memory_copy(owner, std::ptr::null(), 0, &mut id), 0);
        assert_eq!(re_memory_read(owner, id, std::ptr::null_mut(), 0, &mut length), 0);
        assert_eq!(length, 0);
        re_memory_drop(owner);
    }
}
