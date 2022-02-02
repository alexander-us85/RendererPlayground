(def shaders-directory
  (string/join @((os/cwd) "\\" "shaders")))

(def glsc "C:/VulkanSDK/1.2.198.1/Bin/glslc")

(defn is-folder (path)
  (try
    (do (os/dir path) true)
    ((err) nil)))

(defn process-directory-recursively (path process-function)
  (def contents (os/dir path))
  (def full-path (string/join @((os/realpath path) "\\")))
  (loop (item :in contents)
    (def node (string/join @(full-path item)))
    (if (is-folder node)
      (process-directory-recursively node process-function)
      (apply process-function node []))))

(defn delete-shader-binary (shader-path)
  (if (= (last (string/split "." shader-path)) "spv")
    (do
      (print "Deleting shader binary: " shader-path)
      (os/rm shader-path))))

(defn compile-shader (shader-path)
  (print "Compiling shader: " shader-path)
  (def shader-binary-name (string/join @(shader-path ".spv")))
  (def compile-command (string/join @(glsc " -o " shader-binary-name " " shader-path)))
  (os/shell compile-command))

(process-directory-recursively shaders-directory delete-shader-binary)      
(process-directory-recursively shaders-directory compile-shader)