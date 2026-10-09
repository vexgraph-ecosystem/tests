//! Compiled by the registered runner for intended unsafe/arity/type/borrow failures.
use relational_engine_scratchpad::MappedFile;

fn main() {
    #[cfg(unsafe_admission)]
    let _owner = MappedFile!("fixture");
    #[cfg(unsafe_explicit)]
    let _owner = MappedFile::open("fixture", true);
    #[cfg(arity)]
    let _owner = MappedFile!("fixture", true, 1);
    #[cfg(wrong_mode)]
    let _owner = unsafe { MappedFile!("fixture", 1) };
    #[cfg(wrong_path)]
    let _owner = unsafe { MappedFile!(12u32) };
    #[cfg(close_borrow)]
    {
        let mut owner = MappedFile!();
        let bytes = owner.as_slice();
        owner.close();
        println!("{bytes:?}");
    }
    #[cfg(write_borrow)]
    {
        let mut owner = MappedFile!();
        let bytes = owner.as_slice();
        let _ = owner.write(0, &[1]);
        println!("{bytes:?}");
    }
    #[cfg(drop_borrow)]
    {
        let owner = MappedFile!();
        let bytes = owner.as_slice();
        drop(owner);
        println!("{bytes:?}");
    }
    #[cfg(double_mut)]
    {
        let mut owner = MappedFile!();
        let first = owner.as_mut_slice();
        let second = owner.as_mut_slice();
        println!("{first:?} {second:?}");
    }
}
