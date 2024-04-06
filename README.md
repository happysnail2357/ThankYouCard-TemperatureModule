# Assignment 3

The purpose of this assignment is to demonstrate a solution to two different parallel processing problems.

## Problem 1: The Birthday Presents Party

The Minotaur received lots of gifts during his birthday party. He has his servants help him write Thankyou-Cards. He devised the following procedure:
1. The servants will remove gifts from the bag of gifts and insert them into a "linked list" of gifts ordered by the numbers on their tags.
2. Write a Thankyou-Card for a gift in the "linked list" and then remove that gift from the list.
3. Search for a gift with a certain number in the "linked list" without altering the list.

Also, because he is impatient, the Minotaur had his servants alternate between adding gifts to the list and writing Thankyou-Cards (removing gifts).
Unfortunately, after they finished, there were fewer Thankyou-Cards than gifts!

## Problem 2: Atmospheric Temperature Reading Module

Develop an Atmospheric Temperature Reading Module for a future Mars rover. There are eight temperature sensors (controlled by eight threads) that
take temperature readings every minute. At the end of every hour, the module should generate a report with the following information:
- The five highest temperatures and the five lowest temperatures for the previous hour
- The ten minute interval where the largest change in temperature was observed

Data storage and retrieval should be handled carefully, as to not delay a sensor in taking its reading.

## Solutions

To implement solutions to both problems, I implemented a concurrent linked list class that uses fine grained synchronization. During insert and delete, 
a thread must start at the head of the list and obtain and release the locks of the nodes it traverses in a "hand-over-hand" fashion. Using this method 
multiple threads can traverse the list simultaneoulsy while having exclusive access to the nodes they modify. Starvation is not possible since every 
thread must obtain the locks in order and every lock in front of the thread will eventually become available.

### Problem 1

The servants may have ended up with too few Thankyou-Cards because they interfered with each others work. For example while one servant was removing a 
gift that they had finished a Thankyou-Card for, another servant may have been inserting a gift next to it, and in the rush of the moment the inserted 
gift might have been connected to the gift that was just removed and, thus, not properly inserted. To fix this each servant could "lock" the section they 
are working on so the others cannot accidently interfere.

### Problem 2

The temperature module was designed to have eight threads generating temperature values and saving them to a shared memory location in the form of a concurrent 
linked list. At the end of every "hour" the memory of the linked list is moved to a different list object to be processed so further temperature readings could 
continue. When the report is being created, the eight different temperature values for each minute are averaged so there is a single temperature value for each 
minute of the simulation. The high/low temperatures and the greatest difference interval are obtained with the 60 averaged values.
A thread sleep function was used to delay temperature reading and report generation to make the output easier to follow.

## Build Instructions

Make sure that a c++ compiler (g++) is installed on you device, and then execute the following (Linux) commands in a terminal:

### Problem 1

To compile...

$ `g++ ThankyouCard/*.cpp -o cardsim`

To run...

$ `./cardsim`

### Problem 2

To compile...

$ `g++ TemperatureModule/*.cpp -o roversim`

To run...

$ `./roversim`

**Note**: to stop execution, press the *Enter* key

