import os

#https://pythonexamples.org/python-get-list-of-all-files-in-directory-and-sub-directories/
def GetAllFiles():
	path = os.getcwd()
	filelist = []

	for root, dirs, files in os.walk(path):
		for file in files:
			filelist.append(os.path.join(root,file))
	return filelist

def main():
	#Step 1, get all the files in the current working directory
	files = GetAllFiles()

	lines = 0
	cppCount = 0
	hedCount = 0
	skpCount = 0

	_min = ("null", 999999)
	_max = ("null", -1)

	for thisFile in files:
		#Check if this is a valid file to check
		fileValid = False
		if (thisFile.endswith(".cpp") or thisFile.endswith(".c")):
			fileValid = True
			cppCount += 1
		elif (thisFile.endswith(".h")):
			fileValid = True
			hedCount += 1
		else:
			skpCount += 1

		if (fileValid):
			thisFileLines = sum(1 for line in open(thisFile))

			if (thisFileLines > _max[1]):
				_max = (thisFile, thisFileLines)
			elif (thisFileLines < _min[1]):
				_min = (thisFile, thisFileLines)

			lines += thisFileLines

	avg = float(lines / (hedCount + cppCount))
	print(f"Found {cppCount} cpp files\nFound {hedCount} header files\nFound {skpCount} non C++ files\nCounted {lines} lines\nAverage line count {round(avg, 2)} per file\nMin file was {_min[0]} with {_min[1]} lines\nMax file was {_max[0]} with {_max[1]} lines")

if __name__ == "__main__":
	main()