import os
import openai
openai.api_key = os.getenv("OPENAI_API_KEY")

prompt = """
writing a program in C++ which solve the 01 pakage problem
"""

response = openai.Completion.create(
              model="text-davinci-003",  # 使用的模型
              prompt=prompt,   # 生成结果的触发指令
              max_tokens=1000,  # 生成的最大token数量
              temperature=1    # 采样策略， 接近 1 的值会给模型带来更多风险/创造力，而接近 0 的值会生成明确定义的答案。
            )
# print(response)
print(response['choices'][0]['text'])