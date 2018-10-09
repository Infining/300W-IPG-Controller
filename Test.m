a=1
b=2
c=a+b
d=cos(a)
e=a*b

a=[1 2 3 4]
a=[1 2 3; 4 5 6; 7 8 10]
z=zeros(5,5)
a+10
sin(a)
a'
a

% changing format 
p=a*inv(a)
format long
p=a*inv(a)
format short
p=a*inv(a)

% element wise operations
a.*a
a.^3

% concatination
A=[a,a]
A=[a;a]

% complex numbers
sqrt(-1)
% use either i or j for complex number
c=[3+4i, 4+3j; -1, 10]

% array indexing
A=magic(4)
A(4,2) % row,column traversal
A(8) % column traversal
A(1:3,2) % multiple values in a row or column
A(4,:) % an entire row
A(:,3) % an entire column

% equally space vector with an interval
B=0:10:100

% character strings
myText = 'Hello, World!';
otherText = 'You''re right'; % use two single quotes within a string
whos myText
longText = [myText,' - ',otherText]

% converting numbers to strings
f = 71;
c = (f-32)/1.8;
tempText = ['Temperature is ',int2str(c),'C']

% matlab functions
A=[1 3 5];
max(A)
B=[10 5 6]
max(A,B)

% line plots
x = 0:pi/100:2*pi;
y = sin(x);
plot(x,y,'r--')
xlabel('x')
ylabel('sin(x)')
title('Plot the Sine Functions')
