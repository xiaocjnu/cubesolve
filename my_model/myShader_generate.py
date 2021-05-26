# 用来生成着色器的头文件 myShader.h, 如果修改了着色器 xxx.txt, 手动运行此文件生成代码




input_filenames = {'vertexShader_es11' : 'glsl/vertexShader_es11.glsl',
	'fragmentShader_es11': 'glsl/fragmentShader_es11.glsl' }


output_filename = 'myShader.cpp'
output_filename_h = 'myShader.h'



with open(output_filename_h,'w') as fo:
	print('打开输出文件成功')
	fo.write("// " + output_filename_h)
	fo.write("// 这是一个自动生成的文件,请不要编辑!!!\n\n\n")
	fo.write('namespace my\n{\n')

	for (key,fn) in input_filenames.items():
		fo.write('\textern const char* ' + key + ';\n')
		
			
	fo.write('}\n')
	
	
with open(output_filename,'w') as fo:
	print('打开输出文件成功')
	fo.write("// " + output_filename)
	fo.write("// 这是一个自动生成的文件,请不要编辑!!!\n\n\n")
	fo.write("#include\"myShader.h\"\n\n\n")


	fo.write('namespace my\n{\n')

	for (key,fn) in input_filenames.items():
		with open(fn, 'r') as fi:
			print('打开', fn, '成功')
			
			fo.write('\tconst char* ' + key + ' =')
			
			for line in fi.readlines():
				
				line2 = []
				for a in line:
					if a == '"':
						a = r'\"'
					elif a == '\n':
						a = r'\n'
					
					line2.append(a)
				
				line2 = ''.join(line2)
				
				if len(line)>0:
					fo.write('\n\t\t"')
					fo.write(line2)
					fo.write('"')
			fo.write(';\n\n\n')
			
	fo.write('}\n')
    
    
    