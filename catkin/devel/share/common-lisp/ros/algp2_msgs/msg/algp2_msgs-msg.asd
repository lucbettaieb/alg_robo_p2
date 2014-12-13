
(cl:in-package :asdf)

(defsystem "algp2_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Map2D" :depends-on ("_package_Map2D"))
    (:file "_package_Map2D" :depends-on ("_package"))
  ))