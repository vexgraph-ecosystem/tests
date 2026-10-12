from support import NativeOwner
import unittest
class EntryOwner(NativeOwner):
    tag = "invalid"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
