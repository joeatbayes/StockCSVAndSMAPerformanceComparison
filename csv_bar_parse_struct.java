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
class Bar 
{
  public String dTime;
  public String day;
  public float open;
  public float close;
  public float high;
  public float low;
  public long volume;
}
public class csv_bar_parse_struct
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
  public static float averageS(Bar[] bars)
  {
    double total = 0.0;
    for (Bar aBar : bars)
    {
      total += aBar.close;
    }
    return (float) (total / bars.length);
  }
  public static void TestWithStruct(String fiName)
  {
    try
    {
      System.out.println("TEST WITH STRUCT ARR");
      Path p = Paths.get(fiName);
      System.out.println("Size : "  + Files.size(p));
      long start = currms();
      long startRun = start;
      List<String> lines = Files.readAllLines(p, Charset.forName("US-ASCII"));
      elap("read File lines", start);
      int numRow = lines.size() - 1;
     start = currms();
     Bar[] bars = new Bar[numRow]; 
     elap("Create Empty Bar Arr", start);
      start = currms();
      Iterator iterator = lines.iterator();
      String headLine = (String) iterator.next(); 
      int ndx = 0;
      while(iterator.hasNext())
      {
        String aRow = (String) iterator.next(); 
        if (aRow.length() < 5)
          continue;
        Bar aBar = new Bar();
        String[] strArr = aRow.split(",");
        aBar.dTime = strArr[0];
        aBar.day = strArr[1];
        aBar.open = Float.parseFloat(strArr[2]);
        aBar.close = Float.parseFloat(strArr[3]);
        aBar.high = Float.parseFloat(strArr[4]);        
        aBar.low = Float.parseFloat(strArr[5]);
        aBar.volume = Long.parseLong(strArr[6]);        
        bars[ndx] = aBar;
        ndx++;
      }
      elap("Finished ieterator", start);
      elap("Finished complete load ", startRun);
      start = currms();
      float tavg = averageS(bars);
      elap("Calc Average ", start);
      System.out.println("average close = " + tavg);
    } 
    catch (IOException e) 
    {
            System.err.println("Error: " + e.toString());
    }
  } // end Test with Vectors 
  public static void main(String[] args)
  {    
    TestWithStruct("c:\\JOE\\stock\\JTDATA\\symbols\\SPY\\2012.M1.csv");
  }
}