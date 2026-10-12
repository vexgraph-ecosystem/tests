from support import NativeOwner
import unittest
class WebsearchOwner(NativeOwner):
    tag = "websearch"
del NativeOwner
if __name__ == "__main__":
    unittest.main()
