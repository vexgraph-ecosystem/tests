from support import NativeOwner
import unittest
class CreateOwner(NativeOwner):
    tag = "create"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
