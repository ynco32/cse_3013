echo 'Working directory'
read dirname

if [ -n "$dirname" ]
then

        if [ -d "$dirname" ]
	then
		
		if [ -x "$dirname" ]
		then
			 cd $dirname
        
	        else
			echo "access error"
			exit 1
		fi                
	else
                echo 'no such dir'
                exit 1
                
                
        fi
else
        echo "program starts in the current directory"
fi

for dir in *
        do
        newname=`echo $dir | tr '[a-z][A-Z]' '[A-Z][a-z]'`
        mv $dir $newname
done

