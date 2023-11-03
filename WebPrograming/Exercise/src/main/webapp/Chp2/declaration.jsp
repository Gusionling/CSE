<html>
<head>
<title>Scripting Tag</title>
</head>

<body>
	<%! String hello = "Hello,Java Server Pages";
		String getString(String s){
			return s;
		}
	%>
	
	<%= getString(hello) %>
</body>
</html>