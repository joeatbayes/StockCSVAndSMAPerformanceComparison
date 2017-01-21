 

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.Calendar;
import java.util.Date;
import System;

public class Class1 {
 
public long currms() 
{
  return System.currentTimeMillis();
  //return new Date().getTime();
}

 
/* WARNING: NOT FINISHED */
public static void main(String[] args)
{
 
  try
  {
  FileInputStream in = new FileInputStream("2014.M1.csv");
  BufferedReader br = new BufferedReader(new InputStreamReader(in));
  String strLine;
  String[] splits = strIn.split("\n");
  for(String asset: assetClasses)
  {
    System.out.println(asset);
  }
}
 
 
  while((strLine = br.readLine())!= null)
  {
   System.out.println(strLine);
  }
 
  }catch(Exception e){
   System.out.println(e);
  }
 
 }
}