echo "===============build logdriver==============="
cd logdriver;
make -B;
cd -;

echo "===============build liblog==============="
cd liblog;
make -B;
cd -;

echo "===============build logcat==============="
cd logcat;
make -B;
cd -
