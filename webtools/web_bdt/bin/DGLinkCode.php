<?php

foreach (getallheaders() as $name => $value) {
    if("Name" == $name) {   
        $Name = $value;
    }
    if("Password" == $name) {
        $Password = $value;
    }
    if("Mode" == $name) {
        $Mode = $value;
    }
}  

if("TELINK" == $Name && "TELINK" == $Password) {
    if("1" == $Mode) {
        function GetRandStr($length) {
            $str = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
            $len = strlen($str) - 1;
            $randstr = '';
            for ($i = 0; $i < $length; $i++) {
                $num = mt_rand(0,$len);
                $randstr .= $str[$num];
            }
            return $randstr;
        }

        $number = '';
        for($k = 0; $k < 10; $k++) {
            $data = GetRandStr(10)."\r\n";
            $number .= $data;
        }
        echo $number;
    
        $LinkFile = "link";
        file_put_contents($LinkFile,$number,FILE_APPEND); // it moves to the end of the file
        $str = '';
        $LineNum = 0;
        $handle = fopen($LinkFile,"rb+");
        while(!feof($handle)) {     // check whether the end of the file has been reached
            $LineNum++;            // the number of line
            $str = fgets($handle); // read a line
            if("\r\n" == $str) {
                $LineNum--;
            }
        }
        $LineNum = $LineNum - 1;
        echo "===========Total:".$LineNum."===========";
        fclose($handle);
    }
    else if("2" == $Mode) {
        $LineNum = 0;
        $LinkFile = "link";
        $handle = fopen($LinkFile,"rb+");
        while(!feof($handle)) {     // check whether the end of the file has been reached
            $LineNum++;             // the number of line
            $str = fgets($handle);  // read a line
            if("\r\n" == $str) {
                $LineNum--;
            }
            else{
                echo $str;
            }
        }
        $LineNum--;
        fclose($handle);
        echo "===========Total:".$LineNum."===========";
    }
    else if("3" == $Mode) {
        $LinkFile = "link";
        $handle = fopen($LinkFile,"rb+");
        file_put_contents($LinkFile,"");
        fclose($handle);
        echo "===========The file has been cleared===========";
    }
}
else { 
    echo "Name or Password error!";
}
?>
