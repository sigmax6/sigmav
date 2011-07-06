#!/usr/bin/python
################################
#PYTHON TEMPLATE FOR SIGMAX6
#
#
#
################################

#print("Helloworld!")
print("Welcome!")
g = input("Please input your number: ")
guess = int(g)

if guess == 5:
    print("You win!")
else:
    if guess > 5:
        print("Too big!")
    else:
        print("Too low!")

print("Game over!")

