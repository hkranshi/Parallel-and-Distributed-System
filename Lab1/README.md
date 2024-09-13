# Lab 1: Concurrent Programming
Due January 21, 2024 11:59 PM
Instructions
Write a concurrent server to book classrooms.
Bookings will be done for a day for 5 classrooms.
Timeslots are of 1.5 hours starting from 08:00 - 20:00. They do not overlap.
Cancel request will fail if made within 20 seconds from the time of booking.
In the test case, every second one request is made. The Request Time column is the relative time in seconds from start of program.
Status Codes
0  - OK
-1- Slot Already booked
-2- Cancel request received within the cool down period.
-3- Invalid Request

The get request in the sample outputs also outputs all the booked slots.

Updates:
As requested by some of you, I have added leading zero in the timeslots (i.e. 8:00 to 08:00). If you have used the version without leading zero and is time consuming to change the code, you can contact me for test cases without the leading zero.

Additional test cases have been added for you to verify the program.

If you have any queries of find any issues feel free to contact me or drop a comment in the general channel.
 
