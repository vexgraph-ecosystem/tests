// Owner proof for text/string; byte lengths, not character arrays.
use relational_engine_scratchpad::{bytes, text::string};

#[test]
fn byte_text_contract() {
    assert_eq!(string::to_byte_array("hello"), bytes!("hello"));
    assert_eq!(string::as_text(bytes!("🌍")).unwrap(), "🌍");
    assert_eq!(string::as_text(b"").unwrap(), "");
    assert!(string::as_text(&[0xff]).is_err());
    assert_eq!(relational_engine_scratchpad::string::to_byte_array("hello"), b"hello");
}
