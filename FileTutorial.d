/*
 * This is a short tutorial on the file creation system
 * for the D programming language.
 *
 */
import std.stream;
import core.stdc.stdio;
 void main()
 {
   int a = int.sizeof;
   long b = long.sizeof;
   //Create a File stream.
   File firstFile = new File;
   
   //Creates the file with the name Foo.txt.1
   firstFile.create("Foo.txt");
   printf("The file Foo.txt has been created\n");
   
   //Now lets write some stuff to the file.
   firstFile.writeString("This writes a string to the file. ");
   firstFile.writeString("Notice that this will write to the same line. ");
   firstFile.writeLine("This will continue to be on the same line. ");
   firstFile.writeLine("However, this will be on a new line. ");

   firstFile.writeString("The size of an integer is: ");
   firstFile.write(a);
   firstFile.writeLine("");
   firstFile.writeString("The size of a long is: ");
   firstFile.write(b);
   firstFile.writeLine("");
   printf("%d%ld\n", a, b);
   
   printf("Closing the file\n");
   
   //Destructor should close a file if its still open, 
   //but it is best to close the file manually anyway.
   firstFile.close();

   //Create a new File stream and give it
   //a name of a file to open in read only mode.
   try {
      File newFile = new File("Foo.txt");
      printf ("The first line of the file we just created is: \n %.*s\n", newFile.readLine());
      
      //Skips over 10 character from current position.
      newFile.seek(10, SeekPos.Current);
                
      //print out the next 5 characters in the file.
      printf ("The next 5 characters are: %.*s\n", newFile.readString(5));

      //fileStreamName.position tells where in the File stream you are currently at.
      printf ("We are currently at %d in the file \n", newFile.position);

      //We can also adjust our position in the file by using position(ulong).
      //We can seek from the end of the file using seekEnd(long offset), and from the
      //begining using seekSet(long offset)
      newFile.position(newFile.seekEnd(-20));
      printf ("Print the last 20 characters in the file: %.*s\n", newFile.readString(20));

      //one last operation is copying from one file to another.
      File copyFile = new File;
      copyFile.create("FooCopy.txt");
      copyFile.copyFrom(newFile);

      //You can also copy small segments of data from one file to another.
      //The second parameter is a uint of the number of bytes to copy from the first stream.
      //Currently not correctly implemented
      copyFile.copyFrom(newFile, 130);
      
      //Again, close any streams after opening.
      copyFile.close();
      newFile.close();
   } catch(OpenException e) //Catches the exception if the file was not there.
   {
      printf("Sorry, the file was not found");
      return;
   }
 }