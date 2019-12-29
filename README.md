# 俄罗斯方块项目报告

## 分工

| 学号     | 姓名   | 分工         | 占比 |
| -------- | ------ | ------------ | ---- |
| 11712531 | 张婷婷 | coder/tester | 34%  |
| 11712325 | 张家毓 | coder/tester | 33%  |
| 11610615 | 郭孟维 | coder/tester | 33%  |



## 模块设计



## 具体实现


<br>



## 主要方法

基于整体模块架构，我们设计编写了以**draw_model_x**,  **judge**, **fill_record**, **showStatus** 等方法为核心的源码程序.

接下来，我们将分别对这四个核心方法进行具体分析。



### 1.draw_model_x(以draw_model_1为例)



#### I.方法结构

*void draw_model_1(int lor);*



#### II.方法功能

此方法将根据输入的参数lor，对于当前下落方块的行为进行判断（左移，右移，变形，下降）。并在屏幕上绘制出执行对应行为后的方块图形。

- **lor = -1:**  左移操作
- **lor = 0:**  下移操作
- **lor = 1:**  右移操作
- **lor = 2:**  变形操作



#### III.方法实现

正如前文所述，我们的设计思路是使用两个全局变量来表示当前下落方块在整个屏幕中的位置。因此，draw_model这一系列方法首先需要根据不同形状方块的特征来把方块的中心坐标转化为对应该方法的图形。即将xoffset与drop两个变量转化(x1, y1), (x2, y2），如方块由多个部件组成，则分别一一转化。

转换坐标完毕后，根据lor的值，在judge函数通过的情况下更新xoffset与drop两个位置值。如果命令是旋转，则基于新的转换规则获取新的图形绘制区域。

最后，基于更新的xoffset与drop值，更新获得新的图形绘制区域，通过**LCD_Fill**方法绘制方块。



#### IV.方法源码

```c
void draw_model_1(int lor)
{

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    // last state
    if (rotation % 2 == 0)
    {
        x1 = 60 + xoffset;
        y1 = 0 + drop;
        x2 = 140 + xoffset;
        y2 = 20 + drop;
    }
    else
    {
        x1 = 100 + xoffset;
        y1 = 0 + drop;
        x2 = 120 + xoffset;
        y2 = 80 + drop;
    }

    // erase the previous state



    // judge the next state

    // judge left
    if (lor == -1) {
		if(judge(x1-20, y1, x2-20, y2) == 1) {
			xoffset -= 20;
		}else{

		}
    }
    // judge right
     if (lor == 1) {
		if(judge(x1+20, y1, x2+20, y2) == 1) {
			xoffset += 20;
		}else{

		}
    }
    // judge drop
    if (lor == 0) {
    	// if next state is legal
    	if (judge(x1, y1+speed, x2, y2+speed) == 1) {
    		drop += speed;
    	// if next state is illegal
    	}else{

    		if_bottom = 1;
    		fill_record(x1, y1, x2, y2,1);
    	//	LCD_Fill(x1,y1,x2,y2,YELLOW);
    	//	return ;
    	}
    }
   // judge rotate
    if (lor == 2) {
    	//LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);

    	int last = rotation - 1;

    	if (last % 2 == 0)
    	{
    		LCD_Fill(60 + xoffset, 0 + drop, 140 + xoffset, 20 + drop, WHITE);
    	}

    	else
    	{
    		LCD_Fill(100 + xoffset, 0 + drop, 120 + xoffset, 80 + drop, WHITE);
    	}

    	if (judge(x1,y1,x2,y2) == 0) {
    		rotation -= 1;
    	}
    }

   // update the next state
	if (rotation % 2 == 0)
	  {
		  x1 = 60 + xoffset;
		  y1 = 0 + drop;
		  x2 = 140 + xoffset;
		  y2 = 20 + drop;
		 }
	 else
	 {
		  x1 = 100 + xoffset;
		  y1 = 0 + drop;
		  x2 = 120 + xoffset;
		  y2 = 80 + drop;
	 }


	// draw the block
	if (if_bottom == 0) {
		if ( lor == -1 && judge(x1, y1, x2, y2) == 1) {
			LCD_Fill(x1+20, y1, x2+20, y2, WHITE);
		} else
		if (lor == 1 && judge(x1, y1, x2, y2) == 1) {
			LCD_Fill(x1-20, y1, x2-20, y2, WHITE);
		}else if (lor == 0) {
			LCD_Fill(x1, y1-speed, x2, y2-speed, WHITE);
		}

		LCD_Fill(x1, y1, x2, y2, YELLOW);
	}
}
```



<br>


<br>


### 2. judge



#### I.方法结构



#### II.方法功能



#### III.方法实现



#### IV.方法源码



<br>

### 3.fiill_record

#### I.方法结构



#### II.方法功能



#### III.方法实现



#### IV.方法源码

<br>



### 4.showStatus

#### I.方法结构



#### II.方法功能



#### III.方法实现



#### IV.方法源码



<br>



+:
## 结果展示

---

- 右1边状态栏会显示等级和分数，且满行后会消失



![](https://s2.ax1x.com/2019/12/29/lK3sJO.png)

![](https://s2.ax1x.com/2019/12/29/lK3yWD.png)





- 状态栏会显示接下来的两个形状

![](https://s2.ax1x.com/2019/12/29/lK3we1.png)

---
### 计分规则： 每下落一个记1分，消除1行记3分

- level 1（20分以下，速度为2）

![](https://s2.ax1x.com/2019/12/29/lK3Bo6.png)

- level 2（超过20分自动升级level2，速度为5）

![](https://s2.ax1x.com/2019/12/29/lK3cSe.png)

- level3（超过50自动升级为level3，速度为10）

![](https://s2.ax1x.com/2019/12/29/lK3gQH.png)

- 初始形状

![](https://s2.ax1x.com/2019/12/29/lK3ROA.jpg)

- 向左移动

![](https://s2.ax1x.com/2019/12/29/lK3hwt.jpg)

- 向右移动

![](https://s2.ax1x.com/2019/12/29/lK34TP.jpg)

- 旋转后的形状

![](https://s2.ax1x.com/2019/12/29/lK3oY8.jpg)











----





## 未来工作
