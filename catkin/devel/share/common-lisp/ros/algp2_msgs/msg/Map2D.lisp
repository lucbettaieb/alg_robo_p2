; Auto-generated. Do not edit!


(cl:in-package algp2_msgs-msg)


;//! \htmlinclude Map2D.msg.html

(cl:defclass <Map2D> (roslisp-msg-protocol:ros-message)
  ((columns
    :reader columns
    :initarg :columns
    :type (cl:vector std_msgs-msg:Int8MultiArray)
   :initform (cl:make-array 0 :element-type 'std_msgs-msg:Int8MultiArray :initial-element (cl:make-instance 'std_msgs-msg:Int8MultiArray))))
)

(cl:defclass Map2D (<Map2D>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Map2D>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Map2D)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name algp2_msgs-msg:<Map2D> is deprecated: use algp2_msgs-msg:Map2D instead.")))

(cl:ensure-generic-function 'columns-val :lambda-list '(m))
(cl:defmethod columns-val ((m <Map2D>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader algp2_msgs-msg:columns-val is deprecated.  Use algp2_msgs-msg:columns instead.")
  (columns m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Map2D>) ostream)
  "Serializes a message object of type '<Map2D>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'columns))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'columns))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Map2D>) istream)
  "Deserializes a message object of type '<Map2D>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'columns) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'columns)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'std_msgs-msg:Int8MultiArray))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Map2D>)))
  "Returns string type for a message object of type '<Map2D>"
  "algp2_msgs/Map2D")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Map2D)))
  "Returns string type for a message object of type 'Map2D"
  "algp2_msgs/Map2D")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Map2D>)))
  "Returns md5sum for a message object of type '<Map2D>"
  "db42e0320f0430c95b1e0ec660057b45")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Map2D)))
  "Returns md5sum for a message object of type 'Map2D"
  "db42e0320f0430c95b1e0ec660057b45")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Map2D>)))
  "Returns full string definition for message of type '<Map2D>"
  (cl:format cl:nil "# Message to create a vector of vectors to store map data to be sent to the global planner.~%~%std_msgs/Int8MultiArray[] columns~%================================================================================~%MSG: std_msgs/Int8MultiArray~%# Please look at the MultiArrayLayout message definition for~%# documentation on all multiarrays.~%~%MultiArrayLayout  layout        # specification of data layout~%int8[]            data          # array of data~%~%~%================================================================================~%MSG: std_msgs/MultiArrayLayout~%# The multiarray declares a generic multi-dimensional array of a~%# particular data type.  Dimensions are ordered from outer most~%# to inner most.~%~%MultiArrayDimension[] dim # Array of dimension properties~%uint32 data_offset        # padding bytes at front of data~%~%# Accessors should ALWAYS be written in terms of dimension stride~%# and specified outer-most dimension first.~%# ~%# multiarray(i,j,k) = data[data_offset + dim_stride[1]*i + dim_stride[2]*j + k]~%#~%# A standard, 3-channel 640x480 image with interleaved color channels~%# would be specified as:~%#~%# dim[0].label  = \"height\"~%# dim[0].size   = 480~%# dim[0].stride = 3*640*480 = 921600  (note dim[0] stride is just size of image)~%# dim[1].label  = \"width\"~%# dim[1].size   = 640~%# dim[1].stride = 3*640 = 1920~%# dim[2].label  = \"channel\"~%# dim[2].size   = 3~%# dim[2].stride = 3~%#~%# multiarray(i,j,k) refers to the ith row, jth column, and kth channel.~%================================================================================~%MSG: std_msgs/MultiArrayDimension~%string label   # label of given dimension~%uint32 size    # size of given dimension (in type units)~%uint32 stride  # stride of given dimension~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Map2D)))
  "Returns full string definition for message of type 'Map2D"
  (cl:format cl:nil "# Message to create a vector of vectors to store map data to be sent to the global planner.~%~%std_msgs/Int8MultiArray[] columns~%================================================================================~%MSG: std_msgs/Int8MultiArray~%# Please look at the MultiArrayLayout message definition for~%# documentation on all multiarrays.~%~%MultiArrayLayout  layout        # specification of data layout~%int8[]            data          # array of data~%~%~%================================================================================~%MSG: std_msgs/MultiArrayLayout~%# The multiarray declares a generic multi-dimensional array of a~%# particular data type.  Dimensions are ordered from outer most~%# to inner most.~%~%MultiArrayDimension[] dim # Array of dimension properties~%uint32 data_offset        # padding bytes at front of data~%~%# Accessors should ALWAYS be written in terms of dimension stride~%# and specified outer-most dimension first.~%# ~%# multiarray(i,j,k) = data[data_offset + dim_stride[1]*i + dim_stride[2]*j + k]~%#~%# A standard, 3-channel 640x480 image with interleaved color channels~%# would be specified as:~%#~%# dim[0].label  = \"height\"~%# dim[0].size   = 480~%# dim[0].stride = 3*640*480 = 921600  (note dim[0] stride is just size of image)~%# dim[1].label  = \"width\"~%# dim[1].size   = 640~%# dim[1].stride = 3*640 = 1920~%# dim[2].label  = \"channel\"~%# dim[2].size   = 3~%# dim[2].stride = 3~%#~%# multiarray(i,j,k) refers to the ith row, jth column, and kth channel.~%================================================================================~%MSG: std_msgs/MultiArrayDimension~%string label   # label of given dimension~%uint32 size    # size of given dimension (in type units)~%uint32 stride  # stride of given dimension~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Map2D>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'columns) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Map2D>))
  "Converts a ROS message object to a list"
  (cl:list 'Map2D
    (cl:cons ':columns (columns msg))
))
