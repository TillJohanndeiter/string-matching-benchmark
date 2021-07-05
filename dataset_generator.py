import random
import re
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('textLength', type=int)
parser.add_argument('patternLength', type=int)
parser.add_argument('patternInText', type=str)
parser.add_argument('numSamples', type=int)
parser.parse_args()

SPLIT_PATTERN_TEXT = 'SPLIT_PATTERN_TEXT'


def getRandomQuote():
    text_start_idx = random.randrange(start=0, stop=len_bibel)
    text = bible_text[text_start_idx: text_start_idx + text_length + 1]
    return text


if __name__ == '__main__':
    args = parser.parse_args()

    text_length = args.textLength
    pattern_length = args.patternLength
    pattern_in_text = args.patternInText
    num_samples = args.numSamples

    text_to_pattern = []

    with open('bibel.txt') as file:
        bible_text = file.read().replace('\n', '')
        bible_text = re.sub(' +', ' ', bible_text)
        bible_text = bible_text.split(' ')
        len_bibel = len(bible_text)

        while len(text_to_pattern) < num_samples:
            text = getRandomQuote()

            if pattern_in_text == 'True':

                pattern_start_idx = random.randrange(start=0,
                                                     stop=len(bible_text) - pattern_length)

                pattern = ' '.join(
                    bible_text[pattern_start_idx: pattern_start_idx + pattern_length])

                text_to_pattern.append((' '.join(text), pattern))

            else:
                patternNotFound = True

                while patternNotFound:
                    other_text = getRandomQuote()
                    pattern_start_idx = random.randrange(start=0,
                                                         stop=len(bible_text) - pattern_length)
                    pattern = ' '.join(
                        bible_text[pattern_start_idx: pattern_start_idx + pattern_length])

                    if pattern not in text:
                        patternNotFound = False
                        text_to_pattern.append((' '.join(text), pattern))

    with open(
            f'patInTxt={pattern_in_text}txtLen={text_length}patLen={pattern_length}n={num_samples}.bd',
            'w') as file:
        for text, pattern in text_to_pattern:
            file.write(f'{text}{SPLIT_PATTERN_TEXT}{pattern} \n')
