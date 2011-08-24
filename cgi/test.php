<?php
      # start the session, and clear any existing values
      session_start();
      $_SESSION['js_count']   = 0;
?>
<!-- main HTML display -->
<HTML>
      <head>
            <title>PHP & Javascript - Example</title>
            <STYLE type='text/css'>
                  /* set Body display properties to make things pretty */
                  body, p, th, td {
                        font-family: Tahoma, Arial, Helvetica, sans-serif;
                        font-size: 11px;
                  }
            </STYLE>
            <SCRIPT type="text/javascript">    
                  function attach_file( p_script_url ) {
                        // create new script element, set its relative URL, and load it
                        script = document.createElement( 'script' );
                        script.src = p_script_url;
                        document.getElementsByTagName( 'head' )[0].appendChild( script );
                  }
            </SCRIPT>
      </head>    
      <body>

            <!-- call external PHP / Javascript file when clicked -->
            <a href="javascript:attach_file( 'javascript.php' )">What time is it?</a>
            <br><br>
            <span id="dynamic_span" />
      </body>
</HTML>
