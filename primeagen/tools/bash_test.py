from support import NativeOwner
import unittest
class BashOwner(NativeOwner):
    tag = "bash"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
