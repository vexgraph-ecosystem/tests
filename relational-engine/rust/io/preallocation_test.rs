//! Procedural backend owner: compile the exact production platform module as a
//! client and exercise real reserve/EOF/allocation query and readonly rejection.
//! No mocked syscall earns physical-allocation proof; Windows rejects explicitly.
#[path = "../../../../ecosystem/repos/relational-engine/rust/src/io/preallocation.rs"]
mod backend;
mod preallocation_error { pub use relational_engine_scratchpad::PreallocationError; }
use std::{fs::{self, File, OpenOptions}, path::PathBuf};

#[test]
fn real_backend_and_readonly_failure() {
    assert!(backend::supported());
    let path: PathBuf = std::env::temp_dir().join(format!("re-reserve-backend-{}", std::process::id()));
    let file = OpenOptions::new().read(true).write(true).create_new(true).open(&path).unwrap();
    assert!(matches!(backend::reserve(&file, 0), Err(preallocation_error::PreallocationError::Length)));
    assert!(matches!(backend::reserve(&file, u64::MAX), Err(preallocation_error::PreallocationError::Length)));
    backend::reserve(&file, 8193).unwrap();
    backend::set_length(&file, 8193).unwrap();
    assert!(backend::allocated_bytes(&file).unwrap() >= 8193);
    assert_eq!(file.metadata().unwrap().len(), 8193);
    drop(file);
    let readonly = File::open(&path).unwrap();
    assert!(matches!(backend::reserve(&readonly, 8193), Err(preallocation_error::PreallocationError::Io(_))));
    assert!(matches!(backend::set_length(&readonly, 1), Err(preallocation_error::PreallocationError::Io(_))));
    assert_eq!(readonly.metadata().unwrap().len(), 8193);
    drop(readonly);
    fs::remove_file(path).unwrap();
}
