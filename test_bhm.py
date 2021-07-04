import unittest
from bhm import NaiveAlgorithm, BoyerMooreAlgorithm

naive_algo = NaiveAlgorithm()


class NaiveTestCase(unittest.TestCase):

    def test_contains(self):
        self.assertTrue(naive_algo.contains("aabbaa", "bba"))

    def test_index(self):
        self.assertEqual([2], naive_algo.index_of("aabbaa", "bba"))

    def test_multiple_index(self):
        self.assertEqual([2, 6], naive_algo.index_of("aabbaabba", "bba"))

    def test_not_contains(self):
        self.assertFalse(naive_algo.contains("aabbaa", "bbc"))


bm_algo = BoyerMooreAlgorithm()


class BMTestCase(unittest.TestCase):

    def test_contains(self):
        self.assertTrue(bm_algo.contains("aabbaa", "bba"))

    def test_index(self):
        self.assertEqual([2], bm_algo.index_of("aabbaa", "bba"))

    def test_multiple_index(self):
        self.assertEqual([2, 6], bm_algo.index_of("aabbaabba", "bba"))

    def test_not_contains(self):
        self.assertFalse(bm_algo.contains("aabbaa", "bbc"))


if __name__ == '__main__':
    unittest.main()
