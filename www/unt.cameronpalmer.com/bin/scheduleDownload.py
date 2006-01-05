
__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2005, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import urllib2, os, sys
sys.path.append("BeautifulSoup-2.1.1/")
from BeautifulSoup import BeautifulSoup

if __name__ == '__main__':
    """Download the PDF class schedules"""
    
    base = 'http://essc.unt.edu/registrar/SOCbydept/'
    url = 'SOCbydeptA.htm'
    datadir = '../data/pdf/'
    html = urllib2.urlopen(base+url).read()
    soup = BeautifulSoup(html)
    for anchor in soup('a'):
        link = anchor['href']
        if link[-4:] == '.pdf':
            fileurl = base+link
            fileout = datadir+link.lower()
            fileout = fileout.replace('%20', '_')
            directory = datadir+link.split('/', 1)[0]
            if not os.path.exists(directory):
	            try:
	            	os.mkdir(directory)
	            except OSError, e:
	            	print e.strerror
            	
            try:
            	handle = os.popen('wget -q -O %s %s' % (fileout, fileurl))
            	handle.close()
            except OSError, e:
            	pass
           	