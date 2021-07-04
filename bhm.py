import math
from collections import defaultdict


class StringMatchingAlgorithm:

    def index_of(self, text: str, pattern: str) -> list:
        raise NotImplementedError('subclasses must override indexOf()!')

    def contains(self, text: str, pattern: str):
        return self.index_of(text, pattern) != []


class NaiveAlgorithm(StringMatchingAlgorithm):

    def index_of(self, text: str, pattern: str):

        start_idxs = []

        for start_idx in range(len(text) - len(pattern) + 1):
            for match_idx in range(len(pattern)):
                if pattern[match_idx] != text[start_idx + match_idx]:
                    break

                if match_idx == len(pattern) - 1:
                    start_idxs.append(start_idx)

        return start_idxs


class BoyerMooreAlgorithm(StringMatchingAlgorithm):

    def index_of(self, text: str, pattern: str):
        '''
        Source https://www.geeksforgeeks.org/boyer-moore-algorithm-for-pattern-searching/
        :param text:
        :param pattern:
        :return:
        '''

        start_idxs = []

        # Preprocess bad_char table to assing rightest occurrence of the character

        bad_char = defaultdict(lambda: -1)

        for i in range(len(pattern)):
            bad_char[pattern[i]] = i

        text_len = len(text)
        pattern_len = len(pattern)

        border_pos = [0] * (pattern_len + 1)
        shift = [0] * (pattern_len + 1)

        self.pre_strong_suffix(shift, border_pos, pattern, pattern_len)
        self.preprocess_case2(shift, border_pos, pattern_len)

        for start_idx in range(text_len - pattern_len + 1):

            match_idx = pattern_len - 1

            # Iterate from right to left while character at match_idx are matching
            while match_idx >= 0 and pattern[match_idx] == text[start_idx + match_idx]:
                match_idx -= 1

            current_char = text[start_idx + match_idx]

            # Match found
            if match_idx < 0:
                start_idxs.append(start_idx)
                # Move text_start_idx to next character aligns with last occurence in pattern  or increase by one to end outer for loop
                start_idx += (max(pattern_len - bad_char[
                    current_char], shift[0]) if start_idx + pattern_len < text_len else 1)
            # Not a match
            else:
                # Move bad character in text aligns with the last occurrence of it in pattern
                # Max prevented negative shift if last occurrence of current_char is on right side
                start_idx += max(1, shift[match_idx + 1], match_idx - bad_char[current_char])

        return start_idxs

    def pre_strong_suffix(self, shift, border_pos: [], pattern: str, pattern_len: int):
        i = pattern_len
        j = pattern_len + 1
        border_pos[i] = j

        # Outer loop
        while i > 0:

            while j <= pattern_len and pattern[i - 1] != pattern[j - 1]:

                if shift[j] == 0:
                    shift[j] = j - 1
                j = border_pos[j]

            i -= 1
            j -= 1
            border_pos[i] = j

    def preprocess_case2(self, shift: [], border_pos: [], pattern_len: int):
        j = border_pos[0]
        for i in range(pattern_len + 1):

            if shift[i] == 0:
                shift[i] = j

            if i == j:
                j = border_pos[j]


class KnuthMorrisPrattAlgorithm(StringMatchingAlgorithm):

    def index_of(self, text: str, pattern: str) -> list:
        start_idxs = []

        return start_idxs


def prefix(muster, laenge):
    # Laenge des gefundenen Prefix
    laengePrefix = -1

    # Der erste Wert ist immer -1
    prefixTabelle = [laengePrefix]

    # solange das Ende des Musters nicht erreicht ist
    for positionImMuster in range(0, laenge):
        # solange sich ein gefundenes Prefix nicht verlaengern laesst, nach einem kuerzeren suchen
        while laengePrefix >= 0 and muster[laengePrefix] != muster[positionImMuster]:
            laengePrefix = prefixTabelle[laengePrefix]

        # an dieser Stelle ist laengePrefix == -1 oder muster[positionImMuster] == muster[leangePrefix]
        laengePrefix = laengePrefix + 1
        # den gefundenen Wert an die prefixTabelle anhaengen
        prefixTabelle.append(laengePrefix)

    return prefixTabelle



if __name__ == '__main__':
    str_test = 'ababcababa'
    prefix('ababcabab', len(str_test))
