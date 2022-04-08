mkdir Release
pushd Release || exit
cmake -DCMAKE_BUILD_TYPE=Release ..
make
popd || exit
printf "\033c"
pushd mkp_instances/instances || exit
rm stats.data
for file in *; do
  echo "$file"
  printf "$file " >>stats.data
  ../../Release/MKP "$file" --random --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --greedy --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --toyoda --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --random --FI --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --greedy --FI --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --toyoda --FI --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --random --BI --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --greedy --BI --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --toyoda --BI --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --random --VND --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --greedy --VND --seed 50 | sed 's/ ms//g' | tr '\n' ' ' >>stats.data
  ../../Release/MKP "$file" --toyoda --VND --seed 50 | sed 's/ ms//g' >>stats.data
done
