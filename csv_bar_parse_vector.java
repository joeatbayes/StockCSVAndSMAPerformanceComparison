import java.io.InputStreamReader;
import java.util.Calendar;
import java.util.Date;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Iterator;
import java.util.List;
import java.nio.file.attribute.BasicFileAttributes;
import java.io.IOException;
public class csv_bar_parse_vector
{
  public static long currms() 
  {
    return System.currentTimeMillis();   
  }
  public static long elap(String msg, long start) 
  {
     long elap = currms() - start;
     System.out.println("elap " + msg + " elap= " + elap);
     return elap;
  }
  public static float average(float[] pin)
  {
    double total = 0.0;
    for (float aVal : pin)
    {
      total += aVal;
    }
    return (float) (total / pin.length);
  }  
  public static void TestWithVectors(String fiName)
  {
    try
    {
      System.out.println("TestWithVectors()");
      Path p = Paths.get(fiName);
      System.out.println("Size : "  + Files.size(p));
      long start = currms();
      long startRun = start;
      List<String> lines = Files.readAllLines(p, Charset.forName("US-ASCII"));
      elap("read File lines", start);
      int numRow = lines.size() - 1;
     start = currms();
     String[] dTime = new String[numRow];
     String[] day = new String[numRow];
     float[] open = new float[numRow];
     float[] close = new float[numRow];
     float[] high = new float[numRow];
     float[] low = new float[numRow];
     long[] volume = new long[numRow];
     elap("Create Empty Vector Arrays", start);
      start = currms();
      Iterator iterator = lines.iterator();
      String headLine = (String) iterator.next(); 
      int ndx = 0;
      while(iterator.hasNext())
      {
        String aRow = (String) iterator.next(); 
        if (aRow.length() < 5)
          continue;
        String[] strArr = aRow.split(",");
        dTime[ndx] = strArr[0];
        day[ndx] = strArr[1];
        open[ndx] = Float.parseFloat(strArr[2]);
        close[ndx] = Float.parseFloat(strArr[3]);
        high[ndx] = Float.parseFloat(strArr[4]);        
        low[ndx] = Float.parseFloat(strArr[5]);
        volume[ndx] = Long.parseLong(strArr[6]);        
        ndx++;
      }
      elap("Finished ieterator", start);
      elap("Finished complete load ", startRun);
      start = currms();
      float tavg = average(close);
      elap("Calc Average ", start);
      System.out.println("average close = " + tavg);
    } 
    catch (IOException e) 
    {
            System.err.println("Error: " + e.toString());
    }
  }
  public static void main(String[] args)
  {
    TestWithVectors("2014.M1.csv");
  }
}