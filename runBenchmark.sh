 
for approach in naive; do
    for dataset in *.bd; do
        for numRun in 1 2 3; do 
            temp=$(echo $dataset| cut -d'.' -f 1)
            ./$approach $dataset > $approach$temp$numRun.br
        done
    done
done