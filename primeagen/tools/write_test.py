from support import NativeOwner
import unittest
class WriteOwner(NativeOwner):
    tag = "write"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
