%:include <iostream>
 
int main(int argc, char *argv<::>) 
<%
    if (argc > 1 and argv<:1:> not_eq '\0') <%
        std::cout << "Hello " << argv<:1:> << '\n';
    %>
%>