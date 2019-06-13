Compresscss is just a simple css compressor

To compile:
```
make
```

To compress:
```
./compress <input file> <output file>
```

For example, with the test.css file, It will compress
```ruby
           body 							{







		padding-top: 									89px   ;
}
/*
 * Un commentaire sur 3 lignes
 */

html {}

.truc {
	padding: 10px;
}

.truc:before {
	content: '/* Contenu méchant !!! */ \' ';
}

.truc                           :before {
        margin:                        789px;
}

.truc                                                                                                   a {
        text-decoration: underline;
}


.trux a span																						{
		width: calc(100%+2px);
}

/* Un commentaire sur une ligne */
```

In:

```ruby
body{padding-top:89px}.truc{padding:10px}.truc:before{content:'/* Contenu méchant !!! */ \' '}.truc :before{margin:789px}.truc a{text-decoration:underline}.trux a span{width:calc(100% + 2px)}
```
