
for text_length in 1000 10000 100000; do 
    for patLen in 2 10 20 50 100; do
        for contains in True False; do
            echo "patInTxt=${contains}txtLen=${text_length}patLen=${patLen}n=10000.bd"
            if  [ ! -f "patInTxt=${contains}txtLen=${text_length}patLen=${patLen}n=10000.bd" ]; then
                echo Create File $contains $text_length $patLen
                python dataset_generator.py $text_length $patLen $contains 10000;
            fi
        done
    done
done