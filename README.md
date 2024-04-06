# Assignment 3

The purpose of this assignment is to demonstrate a solution to two different parallel processing problems.

## Part 1: The Birthday Presents Party

The Minotaur received lots of gifts during his birthday party. He has his servants help him write Thankyou-Cards. He devised the following procedure:
1. The servants will remove gifts from the bag of gifts and insert them into a "linked list" of gifts ordered by the numbers on their tags.
2. Write a Thankyou-Card for a gift in the "linked list" and then remove that gift from the list.
3. Search for a gift with a certain number in the "linked list" without altering the list.

Also, because he is impatient, the Minotaur had his servants alternate between adding gifts to the list and writing Thankyou-Cards (removing gifts).
Unfortunately, after they finished, there were fewer Thankyou-Cards than gifts!

## Part 2: Atmospheric Temperature Reading Module

Develop an Atmospheric Temperature Reading Module for a future Mars rover. There are eight temperature sensors (controlled by eight threads) that
take temperature readings every minute. At the end of every hour, the module should generate a report with the following information:
- The five highest temperatures and the five lowest temperatures for the previous hour
- The ten minute interval where the largest change in temperature was observed

Data storage and retrieval should be handled carefully, as to not delay a sensor in taking its reading.

## Build Instructions

Make sure that a c++ compiler (g++) is installed on you device, and then execute the following (Linux) commands in a terminal:

### Part 1

To compile...

$ `g++ ThankyouCard/*.cpp -o cardsim`

To run...

$ `./cardsim`

### Part 2

To compile...

$ `g++ TemperatureModule/*.cpp -o roversim`

To run...

$ `./roversim`

<u>Note</u>: to stop execution, press the *Enter* key

