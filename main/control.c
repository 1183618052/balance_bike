#if BALANCE_TYPE == 1

extern int disp_type;      //0-��������ʾ���棬1-�ֱ���ʾ����
int focus=0;    //��ǰ���ڵĽ���
                //0-kp��1-kd��2-L��3-Lm��4-V
void focus_plus()       //����+1
{
    focus++;
    if(focus == 5)
        focus = 0;
}
void focus_minus()      //����-1
{
    focus--;
    if(focus == -1)
        focus = 4;
}
void content_plus()     //��ǰѡ�е�����+
{
    switch(focus)
    {
    case 0:     //kp
        balance_data.kp+=0.01;
        break;
    case 1:     //kd
        balance_data.kd+=0.001;
        break;
    case 2:     //L
        balance_data.L+=0.001;
        break;
    case 3:     //Lm
        balance_data.Lm += 0.001;
        break;
    case 4:     //V
        balance_data.V += 0.001;
        break;
    }
}
void content_minus()    //��ǰѡ�е�����-
{
    switch(focus)
    {
    case 0:     //kp
        balance_data.kp-=0.01;
        break;
    case 1:     //kd
        balance_data.kd-=0.001;
        break;
    case 2:     //L
        balance_data.L-=0.001;
        break;
    case 3:     //Lm
        balance_data.Lm -= 0.001;
        break;
    case 4:     //V
        balance_data.V -= 0.001;
        break;
    }
}

void get_control_dat(u8 dat)
{
    //���̿��Ʋ���
    switch(dat)
    {
    case 'u':
        balance_data.balance_angle+=1;
        break;
    case 'j':
        balance_data.balance_angle-=1;
        break;
    case 'i':
        balance_data.kp+=0.01;
        break;
    case 'k':
        balance_data.kp-=0.01;
        break;
    case 'I':
        balance_data.kp+=0.1;
        break;
    case 'K':
        balance_data.kp-=0.1;
        break;
    case 'o':
        balance_data.kd+=0.001;
        break;
    case 'l':
        balance_data.kd-=0.001;
        break;
    case 'O':
        balance_data.kd+=0.01;
        break;
    case 'L':
        balance_data.kd-=0.01;
        break;
    }
    //�ֱ����Ʋ���
    {
        static u8 flag=0;      //0-��ǰ����ʲôҲ���ǡ�1-��ǰ������Lxֵ��2-��ǰ������Lyֵ��3-��ǰ������Rxֵ��4-��ǰ������Ryֵ��
        s8 temp;
        switch(flag)
        {
        case 0:
            if(dat == 150)
                flag = 1;
            else if(dat == 151)
                flag = 2;
            else if(dat == 152)
                flag = 3;
            else if(dat == 153)
                flag = 4;
            else        //������һ��İ�������
            {
                switch(dat)
                {
                case 200:       //1(����)
                    content_plus();
                    break;
                case 202:       //2 Բ
                    disp_type = 1;
                    focus_plus();
                    break;
                case 204:       //3 ��
                    content_minus();
                    break;
                case 206:       //4 ����
                    focus_minus();
                    break;
                case 208:       //��
                    break;
                case 210:       //�Ұ���
                    balance_data.balance_angle = 2;
                    break;
                case 211:       //���ͷ�
                    balance_data.balance_angle = 0;
                    break;
                case 212:       //��
                    break;
                case 214:       //����
                    balance_data.balance_angle = -2;
                    break;
                case 215:       //���ͷ�
                    balance_data.balance_angle = 0;
                    break;
                }
            }
            break;
        case 1: //Lx
            flag = 0;
            temp = (s8)dat;
            if(temp >= -100 && temp <= 100)
            {
            }
            break;
        case 2: //Ly
            flag = 0;
            break;
        case 3: //Rx
            flag = 0;
            break;
        case 4: //Ry
            flag = 0;
            break;
        }
    }
}

#elif BALANCE_TYPE == 2

extern int disp_type;      //0-��������ʾ���棬1-�ֱ���ʾ����
int focus=0;    //��ǰ���ڵĽ���
                //0-kp��1-kd
void focus_plus()       //����+1
{
    focus++;
    if(focus == 2)
        focus = 0;
}
void focus_minus()      //����-1
{
    focus--;
    if(focus == -1)
        focus = 1;
}
void content_plus()     //��ǰѡ�е�����+
{
    switch(focus)
    {
    case 0:     //kp
        balance_data.kp+=0.1;
        break;
    case 1:     //kd
        balance_data.kd+=0.01;
        break;
    }
}
void content_minus()    //��ǰѡ�е�����-
{
    switch(focus)
    {
    case 0:     //kp
        balance_data.kp-=0.1;
        break;
    case 1:     //kd
        balance_data.kd-=0.01;
        break;
    }
}
void get_control_dat(u8 dat)
{
    //���̿��Ʋ���
    switch(dat)
    {
    case 'u':
        balance_data.balance_angle+=1;
        break;
    case 'j':
        balance_data.balance_angle-=1;
        break;
    case 'i':
        balance_data.kp+=0.1;
        break;
    case 'k':
        balance_data.kp-=0.1;
        break;
    case 'I':
        balance_data.kp+=1;
        break;
    case 'K':
        balance_data.kp-=1;
        break;
    case 'o':
        balance_data.kd+=0.01;
        break;
    case 'l':
        balance_data.kd-=0.01;
        break;
    case 'O':
        balance_data.kd+=0.1;
        break;
    case 'L':
        balance_data.kd-=0.1;
        break;
    }
    //�ֱ����Ʋ���
    {
        static u8 flag=0;      //0-��ǰ����ʲôҲ���ǡ�1-��ǰ������Lxֵ��2-��ǰ������Lyֵ��3-��ǰ������Rxֵ��4-��ǰ������Ryֵ��
        s8 temp;
        switch(flag)
        {
        case 0:
            if(dat == 150)
                flag = 1;
            else if(dat == 151)
                flag = 2;
            else if(dat == 152)
                flag = 3;
            else if(dat == 153)
                flag = 4;
            else        //������һ��İ�������
            {
                switch(dat)
                {
                case 200:       //1(����)
                    content_plus();
                    break;
                case 202:       //2 Բ
                    disp_type = 1;
                    focus_plus();
                    break;
                case 204:       //3 ��
                    content_minus();
                    break;
                case 206:       //4 ����
                    focus_minus();
                    break;
                case 208:       //��
                    break;
                case 210:       //�Ұ���
                    break;
                case 211:       //���ͷ�
                    break;
                case 212:       //��
                    break;
                case 214:       //����
                    break;
                case 215:       //���ͷ�
                    break;
                }
            }
            break;
        case 1: //Lx
            flag = 0;
            temp = (s8)dat;
            if(temp >= -100 && temp <= 100)
            {
            }
            break;
        case 2: //Ly
            flag = 0;
            break;
        case 3: //Rx
            flag = 0;
            break;
        case 4: //Ry
            flag = 0;
            break;
        }
    }
}

#endif
