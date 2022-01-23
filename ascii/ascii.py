"""
	The following script is used for changing Hungarian special characters
	into ASCII compatible latin letters.
"""

import sys

filename = input("give filename to ascii-fy:")

ifile = open(filename)

filename = filename.replace(".csv", "-ascii.csv")
ofile = open(filename, "w")

#read input to file
content = ifile.read()

#replace every char into its correspondent ASCII char
content = content.replace('ö', 'o')
content = content.replace('ü', 'u')
content = content.replace('ó', 'o')
content = content.replace('ő', 'o')
content = content.replace('ú', 'u')
content = content.replace('é', 'e')
content = content.replace('á', 'a')
content = content.replace('ű', 'u')
content = content.replace('í', 'i')

content = content.replace('Ö', 'O')
content = content.replace('Ü', 'U')
content = content.replace('Ó', 'O')
content = content.replace('Ő', 'O')
content = content.replace('Ú', 'U')
content = content.replace('É', 'E')
content = content.replace('Á', 'A')
content = content.replace('Ű', 'U')
content = content.replace('Í', 'I')

#write output into file
ofile.write(content)

ifile.close()
ofile.close()