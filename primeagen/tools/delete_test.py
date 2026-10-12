from support import NativeOwner
import unittest
class DeleteOwner(NativeOwner):
    tag = "delete"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
