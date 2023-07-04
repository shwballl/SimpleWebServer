all: compile run

compile:
	g++ main.cpp -o main.exe -lws2_32

run:
	main.exe index.html style.css #script.js #You can connect some files