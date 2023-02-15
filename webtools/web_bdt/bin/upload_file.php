<?php
    header('Access-Control-Allow-Origin:*');
    $LinkFile = "link";
    echo "\n";
    foreach (getallheaders() as $name => $value) {
        if($name == "Name") { // File Name
            echo "$name: $value\n";
            $Name = $value;
        }
        if($name == "Path") { // File Path
            echo "$name: $value\n";
            $Path = $value;
        }
        if($name == "Link") { // Link Code
            echo "$name: $value\n";
            $Link = $value;
        }
    }   
    
    if($Path != NULL && $Name != NULL && $Link != NULL) {
        $ret = LinkCodeCheck($LinkFile, $Link); // check link code
        if(0 == $ret) {
            $input = file_get_contents('php://input'); 
            $dir = "./upload/".$Path;
            if (!file_exists($dir)){
                mkdir ($dir,0777,true);
            }
            $FileLen = file_put_contents($dir."/".$Name, $input);  
            if($FileLen == False) {
                echo "Upload Failed\n";
            }
            else {
                echo "File length:".$FileLen."\n";
                echo "Upload successed  ";
            }
        }
        else {
            echo "Link Code error";
        }
    }
    else {
        echo "Upload Failed\n";
    }
   
    function DeleteFileLine($File,$Number) {
        $handle = fopen($File,'r');
        $str  = '';
        $loop = 0;
        while(!feof($handle)) {
            $line = fgets($handle);
            $loop++;
            if($loop == $Number) {
                continue;
            }
            else {
                $str .= $line;
            }
        }
        fclose($handle);
        $handle = fopen($File,'w');
        file_put_contents($File,$str);
        fclose($handle);
    }

    function LinkCodeCheck($LinkFile, $LinkCode) {
        $str = '';
        $LineNum = 0;
        $handle  = fopen($LinkFile,"rb+");
        if($handle) {                  // file is valid
            while(!feof($handle)) {    // check whether the end of the file has been reached
                $LineNum++;            // the number of line
                $str = fgets($handle); // read a line
                 if(0 == strncmp($LinkCode,$str,10)) { // find the code and delete it
                    fclose($handle);
                    DeleteFileLine($LinkFile,$LineNum);
                    return 0;
                } 
            }
            fclose($handle);
            return -1;
        }
        else{
            fclose($handle);
            return -2;
        }
        return -3;
    }
?>


