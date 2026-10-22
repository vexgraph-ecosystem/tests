from support import NativeOwner
import unittest
class ReadOwner(NativeOwner):
    tag = "read"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
