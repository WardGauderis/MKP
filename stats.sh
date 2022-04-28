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
  ../../Release/MKP "$file" --SA >>stats.data
  printf "\n" >>stats.data
done
