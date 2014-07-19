CXXFLAGS=-g -Istub `pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`

test: test.o jni_part.o MotorControl.o
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o test
