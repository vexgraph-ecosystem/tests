from support import NativeOwner
import unittest
class EditOwner(NativeOwner):
    tag = "edit"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
