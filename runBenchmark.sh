 
for approach in naive; do
    for dataset in *.bd; do
        for numRun in 1 2 3; do
            temp=$(echo $dataset| cut -d'.' -f 1)
            if  [ ! -f $approach$temp$numRun.br ]; then
                echo created $approach$temp$numRun.br
                ./$approach $dataset > $approach$temp$numRun.br
            fi
        done
    done
done