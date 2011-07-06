#!/usr/bin/python
################################
#PYTHON TEMPLATE FOR SIGMAX6
#
#
#
################################
from random import randint
secret = randint(1,10)

print("Welcome!")
g = input("Please input your number: ")
guess = int(g)

while guess != secret:
    if guess > secret:
        print("Too big!")
    else:
        print("Too low!")
    g = input("Please input your number: ")
    guess = int(g)
print("you win!")
print("Game over!")

