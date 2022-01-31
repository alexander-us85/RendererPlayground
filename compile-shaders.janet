(def delete-command "del /s /q ")

(print "Compiling shaders")
(print "-----------------------------------")
(def shader-paths (os/dir "shaders"))
(loop [shader-path :in shader-paths]
  (def path (string/join @["shaders\\" shader-path]))
  (if (= (last (string/split "." shader-path))
         "spv")
    (do
      (print "Deleting " shader-path)
      (os/execute @("cmd.exe" "/c" "cd"))
      #(os/execute @("cmd.exe" "/c" 
      #  (string/join @[delete-command path])
      )))))