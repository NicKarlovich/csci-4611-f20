# repo-karlo015

My "more robust" raindrop selection algorithm goes something like this.

It takes all the words in the entire message; words are defined as being separated by spaces.
Then when it tries to generate a letter or "raindrop" 1/6th  the time it will also do a "word raindrop".
Here there are about 3 random variables in play now which are:
- which word will be generated  
- how many letters from the word will be generated (25% up to 100%)  
- Which letter will be generated each time the system generates another letter of the word

It should be noted that the system DOES know which letters have been generated for any given "word raindrop"

When attempting to generate a word, the system counts how many letters are in the word and then generates letters until this statement is true:

letters_generated/number_of_letters_in_word > number_of_letters_to_generate (2nd random varible listed above)

Once the system can't generate anymore letters; all the newly generated letters are pushed onto the screen and they all fall.  Each of these letters are subject to random velocities in order to still give it the "rain effect".  Regardless, most of the time, a word can be deciphered from the falling letters.
