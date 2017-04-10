#!/bin/bash


{
  rm -f ./*.mxp ./*.log ./*.err
  for file in $(find . -name "*.txt") ; do
    basefile=$(basename "$file" .txt)
    {
      echo "{"
      echo \"receive_from\"                : \"file\",
      echo \"input_file\"                  : \""${file}"\",
      echo \"number_of_inertials_per_gps\" : 1
      echo "}"
    } > rtos_config.json
  
    RTOS
    for mxpfile in $(find . -name "*.mxp") ; do
      cat "$mxpfile" >> unique.mxp
    done 
    if [ -s unique.mxp ] ; then mxpparser -i unique.mxp -o "${basefile}.json" ; fi
    if [ -s "${basefile}.json" ] ; then json_to_html -i "${basefile}.json" -o "${basefile}.html" ; fi
    trip_stats "${basefile}.json"

    rm -f ./*.mxp ./*.log ./*.err "${basefile}"*CALIB*.json "${basefile}"*DIAG*.json "${basefile}"*TRIPSTATS*.txt
    #rm -f *.mxp *.log *.err ${basefile}*.json
  done

  global_stats ./*TRIPSTATS*.json
} 2>&1 | tee -a batch_log 

