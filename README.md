To compress:

./compress \<input file\> \<output file\>

It will compress
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
	content: '/* Contentenu méchant !!! */ \' ';
}

/* Un commentaire sur une ligne */
```

In:

```ruby
body{padding-top:89px}.truc{padding:10px}.truc:before{content:'/* Contentenu méchant !!! */ \' '}
```
