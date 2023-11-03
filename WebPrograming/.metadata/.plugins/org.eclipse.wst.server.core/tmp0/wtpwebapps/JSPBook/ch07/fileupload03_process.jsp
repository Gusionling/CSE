<%@page import="org.apache.commons.fileupload.*"%>
<%@ page contentType = "text/html; charset=utf-8" %>
<%@ page import="java.util.*" %>
<%@ page import="java.io.*" %>
<html>
<head>
<title>File upload</title>
</head>
<body>
	<%
		String fileUploadPath = "F:\\upload";
	
		DiskFileUpload upload = new DiskFileUpload();
		
		List items = upload.parseRequest(request);
		
		Iterator params = items.iterator();
		
		while(params.hasNext()){
			FileItem fileItem = (FileItem) params.next();
			if(!fileItem.isFormField()){
				String fileName = fileItem.getName();
				fileName = fileName.substring(fileName.lastIndexOf("\\") + 1);
				File file = new File(fileUploadPath + "/" + fileName);
				fileItem.write(file);
				
			}
		}
		
	%>
	
</body>
</html>