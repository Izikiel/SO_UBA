def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

from nltk.tokenize.punkt import PunktWordTokenizer
f = file('ej3.out', 'r')
raw = f.read();
count = 0
for x in raw.replace('/', ' ').replace('\'', ' ').replace('}', ' ').split():
  if is_number(x):
    count += 1
    if count % 3 != 0:
      print x, 
    else:
      print x