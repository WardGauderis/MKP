
mkdir Release
pushd Release || exit
cmake -DCMAKE_BUILD_TYPE=Release ..
make
popd || exit
printf "\033c"
pushd mkp_instances/instances || exit

#for file in *; do
#  echo "$file"
#  printf "$file " >>$FILENAME
#  ../../Release/MKP "$file" --SA >>$FILENAME
#  printf "\n" >>$FILENAME
#done

FILENAME="../../data/rtd/SA_1.data"
rm $FILENAME

for f in {1..5}; do
  file="OR10x250-0.25_$f.dat"
  printf "$file " >>$FILENAME
done
printf "\n" >>$FILENAME
for i in {1..25}; do
  echo "$i"
  for f in {1..5}; do
      file="OR10x250-0.25_$f.dat"
    ../../Release/MKP "$file" --SA --seed "$i" >>$FILENAME
    printf " " >>$FILENAME
  done
  printf "\n" >>$FILENAME
done

FILENAME="../../data/rtd/MA_1.data"
rm $FILENAME

for f in {1..5}; do
  file="OR10x250-0.25_$f.dat"
  printf "$file " >>$FILENAME
done
printf "\n" >>$FILENAME
for i in {1..25}; do
  echo "$i"
  for f in {1..5}; do
      file="OR10x250-0.25_$f.dat"
    ../../Release/MKP "$file" --MA --seed "$i" >>$FILENAME
    printf " " >>$FILENAME
  done
  printf "\n" >>$FILENAME
done
