import os
import platform

def comment_include_unistd(file_path):
  #open the file and read its content
  with open(file_path, 'r') as file:
    lines = file.readlines()

  #search for and comment out the '#include <unistd.h>' line
  with open(file_path, 'w') as file:
    for line in lines:
      if '#include <unistd.h>' in line:
        #comment out the line
        file.write('//' + line)
      else:
        file.write(line)
        
  #end function      

def comment_unistd_lexyy(file_path):
  comment_include_unistd(file_path)
  print(f"File {file_path} successfully modified.")

if __name__ == "__main__":
  
  if platform.system() == "Windows":
    #path to the lex.yy.h file
    #file_path = './lex.yy.h'
    #run functions
    comment_unistd_lexyy('./lex.yy.h')
    comment_unistd_lexyy('./lex.yy.c')
    
  else:
    print("This script is intended to run only on windows.")
  

