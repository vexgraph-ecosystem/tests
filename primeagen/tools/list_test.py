from support import NativeOwner
import unittest
class ListOwner(NativeOwner):
    tag = "list"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
