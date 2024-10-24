# MaponHashtable
Creates a simple prime number modulus hash, and uses it to hash the key of key-value pairs being added to the map. 
If a value already exists at the hashed location, the key-value pair is added to the next empty location and then it is linked to the original hashed value, using the 'next' array.
An iterator is also implemented for efficiency.
